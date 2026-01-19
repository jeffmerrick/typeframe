declare module "@theme/IdealImage" {
  import type { ComponentType } from "react";

  export interface IdealImageProps {
    img: {
      src: {
        src: string;
        images: Array<{
          path: string;
          width: number;
          height: number;
        }>;
        placeholder?: string;
      };
    };
    alt?: string;
    className?: string;
  }

  const IdealImage: ComponentType<IdealImageProps>;
  export default IdealImage;
}
